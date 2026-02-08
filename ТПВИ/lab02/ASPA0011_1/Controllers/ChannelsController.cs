using ASPA0011_1.Models;
using ASPA0011_1.Services;
using Microsoft.AspNetCore.Mvc;

namespace ASPA0011_1.Controllers
{
    [ApiController]
    [Route("api/[controller]")]
    public class ChannelsController : ControllerBase
    {
        private readonly IChannelService _channelService;
        private readonly ILogger<ChannelsController> _logger;

        public ChannelsController(IChannelService channelService, ILogger<ChannelsController> logger)
        {
            _channelService = channelService;
            _logger = logger;
        }

        [HttpGet]
        public async Task<IActionResult> GetChannels()
        {
            _logger.LogTrace("GET /api/channels called");

            var channels = await _channelService.GetAllChannelsAsync();

            if (channels.Count == 0)
            {
                _logger.LogDebug("No channels found, returning 204");
                return NoContent();
            }

            _logger.LogDebug("Returning {Count} channels", channels.Count);
            return Ok(channels);
        }

        [HttpGet("{id:guid}")]
        public async Task<IActionResult> GetChannel(Guid id)
        {
            _logger.LogTrace("GET /api/channels/{ChannelId} called", id);

            var channel = await _channelService.GetChannelAsync(id);

            if (channel == null)
            {
                _logger.LogError("Channel {ChannelId} not found, returning 404", id);
                return NotFound();
            }

            _logger.LogDebug("Returning channel {ChannelId}", id);
            return Ok(channel);
        }

        [HttpPost]
        public async Task<IActionResult> CreateChannel([FromBody] CreateChannelRequest request)
        {
            _logger.LogTrace("POST /api/channels called with name {Name}", request.Name);

            if (request.Command != "new")
            {
                _logger.LogError("Invalid command for channel creation: {Command}", request.Command);
                return BadRequest("Command must be 'new'");
            }

            var channel = await _channelService.CreateChannelAsync(request);

            var statusCode = channel.State == "ACTIVE" ? 201 : 204;
            _logger.LogInformation("Created channel {ChannelId} with state {State}, returning {StatusCode}",
                channel.Id, channel.State, statusCode);

            return StatusCode(statusCode, channel);
        }

        [HttpPut]
        public async Task<IActionResult> ChangeChannelStatus([FromBody] ChangeStatusRequest request)
        {
            _logger.LogTrace("PUT /api/channels called with command {Command}", request.Command);

            var channels = await _channelService.ChangeChannelStatusAsync(request);

            _logger.LogDebug("Changed channel status, returning {Count} channels", channels.Count);
            return Ok(channels);
        }

        [HttpDelete]
        public async Task<IActionResult> DeleteChannel([FromBody] DeleteChannelRequest request)
        {
            _logger.LogTrace("DELETE /api/channels called with command {Command}", request.Command);

            var channels = await _channelService.DeleteChannelAsync(request);

            _logger.LogInformation("Deleted channels, returning {Count} remaining channels", channels.Count);
            return Ok(channels);
        }
    }
}