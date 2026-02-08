// IChannelService.cs
using ASPA0011_1.Models;
using Microsoft.Extensions.Logging;
using Microsoft.Extensions.Options;
using System.Collections.Concurrent;
using System.Threading.Channels;

namespace ASPA0011_1.Services
{
    public interface IChannelService
    {
        Task<List<ChannelInfo>> GetAllChannelsAsync();
        Task<ChannelInfo?> GetChannelAsync(Guid id);
        Task<ChannelInfo> CreateChannelAsync(CreateChannelRequest request);
        Task<List<ChannelInfo>> ChangeChannelStatusAsync(ChangeStatusRequest request);
        Task<List<ChannelInfo>> DeleteChannelAsync(DeleteChannelRequest request);
        Task<QueueOperationResponse> ExecuteQueueOperationAsync(QueueOperationRequest request);
    }
}