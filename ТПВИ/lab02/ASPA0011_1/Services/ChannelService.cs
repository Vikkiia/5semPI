

// ChannelService.cs
using ASPA0011_1.Models;
using Microsoft.Extensions.Options;
using System.Collections.Concurrent;
using System.Threading.Channels;

namespace ASPA0011_1.Services
{
    public class ChannelService : IChannelService
    {
        private readonly ConcurrentDictionary<Guid, ChannelWrapper> _channels = new();
        private readonly ILogger<ChannelService> _logger;
        private readonly AppSettings _settings;

        public ChannelService(ILogger<ChannelService> logger, IOptions<AppSettings> settings)
        {
            _logger = logger;
            _settings = settings.Value;
        }

        public Task<List<ChannelInfo>> GetAllChannelsAsync()
        {
            _logger.LogTrace("Getting all channels");
            var channels = _channels.Values.Select(c => c.GetInfo()).ToList();

            if (channels.Count == 0)
            {
                _logger.LogDebug("No channels found");
            }
            else
            {
                _logger.LogDebug("Retrieved {Count} channels", channels.Count);
            }

            return Task.FromResult(channels);
        }

        public Task<ChannelInfo?> GetChannelAsync(Guid id)
        {
            _logger.LogTrace("Getting channel {ChannelId}", id);

            if (_channels.TryGetValue(id, out var channel))
            {
                _logger.LogDebug("Found channel {ChannelId}", id);
                return Task.FromResult<ChannelInfo?>(channel.GetInfo());
            }

            _logger.LogError("Channel {ChannelId} not found", id);
            return Task.FromResult<ChannelInfo?>(null);
        }

        public Task<ChannelInfo> CreateChannelAsync(CreateChannelRequest request)
        {
            _logger.LogTrace("Creating channel with name {Name}", request.Name);

            var channel = new ChannelWrapper(request.Name, request.State, request.Description);
            _channels.TryAdd(channel.Id, channel);

            _logger.LogInformation("Created channel {ChannelId} with name {Name} and state {State}",
                channel.Id, request.Name, request.State);

            return Task.FromResult(channel.GetInfo());
        }

        public async Task<List<ChannelInfo>> ChangeChannelStatusAsync(ChangeStatusRequest request)
        {
            _logger.LogTrace("Changing channel status with command {Command}", request.Command);

            var affectedChannels = new List<ChannelWrapper>();

            if (request.Id.HasValue)
            {
                // Single channel operation
                if (_channels.TryGetValue(request.Id.Value, out var channel))
                {
                    affectedChannels.Add(channel);
                }
            }
            else
            {
                // All channels operation
                affectedChannels.AddRange(_channels.Values);
            }

            foreach (var channel in affectedChannels)
            {
                var oldState = channel.State;

                switch (request.Command.ToLower())
                {
                    case "open":
                        if (channel.State == "ACTIVE")
                        {
                            _logger.LogWarning("Attempting to open already active channel {ChannelId}", channel.Id);
                        }
                        else
                        {
                            channel.State = "ACTIVE";
                            _logger.LogInformation("Channel {ChannelId} opened (was {OldState})", channel.Id, oldState);
                        }
                        break;

                    case "close":
                        if (channel.State == "CLOSED")
                        {
                            _logger.LogWarning("Attempting to close already closed channel {ChannelId}", channel.Id);
                        }
                        else
                        {
                            channel.State = "CLOSED";
                            _logger.LogInformation("Channel {ChannelId} closed (was {OldState}). Reason: {Reason}",
                                channel.Id, oldState, request.Reason);
                        }
                        break;
                }
            }

            return await GetAllChannelsAsync();
        }

        public async Task<List<ChannelInfo>> DeleteChannelAsync(DeleteChannelRequest request)
        {
            _logger.LogTrace("Deleting channels with command {Command}", request.Command);

            var channelsToDelete = _channels.Values.ToList();

            if (request.State == "CLOSED")
            {
                // Delete only CLOSED channels
                channelsToDelete = _channels.Values.Where(c => c.State == "CLOSED").ToList();
                _logger.LogInformation("Deleting {Count} CLOSED channels", channelsToDelete.Count);
            }
            else
            {
                // Delete all channels
                _logger.LogInformation("Deleting all {Count} channels", channelsToDelete.Count);
            }

            foreach (var channel in channelsToDelete)
            {
                _channels.TryRemove(channel.Id, out _);
            }

            return await GetAllChannelsAsync();
        }


        public async Task<QueueOperationResponse> ExecuteQueueOperationAsync(QueueOperationRequest request)
        {
            _logger.LogTrace("Executing queue operation {Operation} on channel {ChannelId}",
                request.Command, request.Id);

            if (!_channels.TryGetValue(request.Id, out var channel))
            {
                _logger.LogError("Channel {ChannelId} not found for queue operation", request.Id);
                return new QueueOperationResponse
                {
                    Id = request.Id,
                    Error = "Channel not found"
                };
            }

            try
            {
                switch (request.Command.ToLower())
                {
                    case "enqueue":
                        if (string.IsNullOrEmpty(request.Data))
                        {
                            return new QueueOperationResponse
                            {
                                Id = request.Id,
                                Error = "Data is required for enqueue operation"
                            };
                        }

                        var success = await channel.EnqueueAsync(request.Data, _settings.WaitEnqueue);
                        if (!success)
                        {
                            _logger.LogWarning("Enqueue operation timed out after {Timeout} seconds on channel {ChannelId}",
                                _settings.WaitEnqueue, request.Id);
                            return new QueueOperationResponse
                            {
                                Id = request.Id,
                                Error = $"Enqueue timeout after {_settings.WaitEnqueue} seconds"
                            };
                        }

                        _logger.LogDebug("Enqueued data to channel {ChannelId}", request.Id);
                        return new QueueOperationResponse { Id = request.Id };

                    case "dequeue":
                        var dequeuedData = await channel.DequeueAsync();
                        if (dequeuedData == null)
                        {
                            _logger.LogCritical("Dequeue operation failed on channel {ChannelId}, no data available to dequeue", request.Id); // Critical error
                            return new QueueOperationResponse
                            {
                                Id = request.Id,
                                Error = "No data available" 
                            };
                        }

                        _logger.LogDebug("Dequeued data from channel {ChannelId}", request.Id);
                        return new QueueOperationResponse
                        {
                            Id = request.Id,
                            Data = dequeuedData
                        };

                    case "peek":
                        var peekedData = await channel.PeekAsync();
                        if (peekedData == null)
                        {
                            _logger.LogDebug("No data to peek from channel {ChannelId}", request.Id);
                            return new QueueOperationResponse
                            {
                                Id = request.Id,
                                Error = "No data available"
                            };
                        }

                        _logger.LogDebug("Peeked data from channel {ChannelId}", request.Id);
                        return new QueueOperationResponse
                        {
                            Id = request.Id,
                            Data = peekedData
                        };

                    default:
                        _logger.LogError("Unknown queue operation {Operation}", request.Command);
                        return new QueueOperationResponse
                        {
                            Id = request.Id,
                            Error = $"Unknown operation: {request.Command}"
                        };
                }
            }
            catch (Exception ex)
            {
                _logger.LogError(ex, "Error executing queue operation {Operation} on channel {ChannelId}",
                    request.Command, request.Id);
                return new QueueOperationResponse
                {
                    Id = request.Id,
                    Error = ex.Message
                };
            }
        }
    }

    public class ChannelWrapper
    {
        private readonly Channel<string> _channel;
        private readonly AsyncQueue<string> _asyncQueue = new();

        public Guid Id { get; } = Guid.NewGuid();
        public string Name { get; }
        public string State { get; set; }
        public string Description { get; }
        public DateTime Created { get; } = DateTime.UtcNow;

        public ChannelWrapper(string name, string state, string description)
        {
            Name = name;
            State = state;
            Description = description;

            var options = new BoundedChannelOptions(1000)
            {
                FullMode = BoundedChannelFullMode.Wait,
                SingleReader = false,
                SingleWriter = false
            };
            _channel = Channel.CreateBounded<string>(options);
        }

        public async Task<bool> EnqueueAsync(string data, int timeoutSeconds)
        {
            if (State != "ACTIVE")
                return false;

            using var cts = new CancellationTokenSource(TimeSpan.FromSeconds(timeoutSeconds));
            try
            {
                await _channel.Writer.WriteAsync(data, cts.Token);
                await _asyncQueue.EnqueueAsync(data); // Also add to async queue for backup
                return true;
            }
            catch (OperationCanceledException)
            {
                return false;
            }
        }

        public async Task<string?> DequeueAsync()
        {
            if (State != "ACTIVE")
                return null;

            if (_channel.Reader.TryRead(out var item))
            {
                await _asyncQueue.DequeueAsync(); // Sync with async queue
                return item;
            }

            return await _asyncQueue.DequeueAsync();
        }

        public async Task<string?> PeekAsync()
        {
            if (State != "ACTIVE")
                return null;

            if (_channel.Reader.TryPeek(out var item))
            {
                return item;
            }

            return await _asyncQueue.PeekAsync();
        }

        public ChannelInfo GetInfo()
        {
            return new ChannelInfo
            {
                Id = Id,
                Name = Name,
                State = State,
                Description = Description,
                Created = Created
            };
        }
    }

    // Simple async queue implementation
    public class AsyncQueue<T>
    {
        private readonly Queue<T> _queue = new();
        private readonly SemaphoreSlim _semaphore = new(0);
        private readonly object _lock = new();

        public async Task EnqueueAsync(T item)
        {
            lock (_lock)
            {
                _queue.Enqueue(item);
            }
            _semaphore.Release();
        }

        public async Task<T> DequeueAsync()
        {
            await _semaphore.WaitAsync();
            lock (_lock)
            {
                return _queue.Dequeue();
            }
        }

        public async Task<T?> PeekAsync()
        {
            if (await _semaphore.WaitAsync(0))
            {
                _semaphore.Release();
                lock (_lock)
                {
                    return _queue.Count > 0 ? _queue.Peek() : default;
                }
            }
            return default;
        }
    }
}