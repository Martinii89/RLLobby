namespace RLLobby.Server.Contracts.Requests;

public class UpdateLobbyReqeust
{
    public Guid Id { get; set; }
    public string Name { get; set; } = default!;
    public string Map { get; set; } = default!;
    public string Description { get; set; } = default!;
    public int PlayerCount { get; set; }
    public List<string> Players { get; set; } = new();
    public bool HasPassword { get; set; }
    public string IpAddress { get; set; } = default!;
    public int Port { get; set; }
    [FromHeader]
    public Guid Token { get; set; }
}