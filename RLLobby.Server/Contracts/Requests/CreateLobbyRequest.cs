namespace RLLobby.Server.Contracts.Requests;

public class CreateLobbyRequest
{
    public string Name { get; set; } = default!;
    public string Map { get; set; } = default!;
    public string Description { get; set; } = default!;
    public int PlayerCount { get; set; }
    public List<string> Players { get; set; } = new();
    public bool HasPassword { get; set; } = false;
    public string? IpAddress { get; set; }
    public int Port { get; set; }
}

