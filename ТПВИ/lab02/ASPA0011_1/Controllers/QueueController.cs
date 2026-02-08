using ASPA0011_1.Models;
using ASPA0011_1.Services;
using Microsoft.AspNetCore.Mvc;

namespace ASPA0011_1.Controllers
{
    [ApiController]
    [Route("api/[controller]")]
    public class QueueController : ControllerBase
    {
        private readonly IChannelService _channelService;
        private readonly ILogger<QueueController> _logger;

        public QueueController(IChannelService channelService, ILogger<QueueController> logger)
        {
            _channelService = channelService;
            _logger = logger;
        }

        [HttpPost]
        public async Task<IActionResult> ExecuteQueueOperation([FromBody] QueueOperationRequest request)
        {
            _logger.LogTrace("POST /api/queue called with operation {Operation} on channel {ChannelId}",
                request.Command, request.Id);

            var result = await _channelService.ExecuteQueueOperationAsync(request);

            if (!result.Success)
            {
                if (result.Error == "Channel not found")
                {
                    _logger.LogError("Channel {ChannelId} not found for queue operation", request.Id);
                    return NotFound(result);
                }

                _logger.LogWarning("Queue operation failed: {Error}", result.Error);
                return BadRequest(result);
            }

            _logger.LogDebug("Queue operation {Operation} completed successfully", request.Command);
            return Ok(result);
        }
    }
}