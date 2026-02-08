// ChannelInfo.cs
namespace ASPA0011_1.Models
{
    public class ChannelInfo
    {
        public Guid Id { get; set; }
        public string Name { get; set; } = string.Empty;
        public string State { get; set; } = "ACTIVE"; // CLOSED | ACTIVE
        public string Description { get; set; } = string.Empty;
        public DateTime Created { get; set; } = DateTime.UtcNow;
    }
}

// CreateChannelRequest.cs
namespace ASPA0011_1.Models
{
    public class CreateChannelRequest
    {
        public string Command { get; set; } = "new";
        public string Name { get; set; } = string.Empty;
        public string State { get; set; } = "ACTIVE"; // CLOSED | ACTIVE
        public string Description { get; set; } = string.Empty;
    }
}

// ChangeStatusRequest.cs
namespace ASPA0011_1.Models
{
    public class ChangeStatusRequest
    {
        public string Command { get; set; } = string.Empty; // open, close
        public Guid? Id { get; set; }
        public string? Reason { get; set; }
        public string? State { get; set; } // CLOSED | ACTIVE
    }
}

// DeleteChannelRequest.cs
namespace ASPA0011_1.Models
{
    public class DeleteChannelRequest
    {
        public string Command { get; set; } = "del";
        public string? State { get; set; } // CLOSED
    }
}

// QueueOperationRequest.cs
namespace ASPA0011_1.Models
{
    public class QueueOperationRequest
    {
        public string Command { get; set; } = string.Empty; // enqueue, dequeue, peek
        public Guid Id { get; set; }
        public string? Data { get; set; } // JSON string for enqueue
    }
}

// QueueOperationResponse.cs
namespace ASPA0011_1.Models
{
    public class QueueOperationResponse
    {
        public Guid Id { get; set; }
        public string? Data { get; set; }
        public string? Error { get; set; }
        public bool Success => string.IsNullOrEmpty(Error);
    }
}

// AppSettings.cs
namespace ASPA0011_1.Models
{
    public class AppSettings
    {
        public int WaitEnqueue { get; set; } = 30; // seconds
    }
}