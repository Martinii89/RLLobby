namespace RLLobby.Server.Models;

public class Lobby
{
    public Guid Id { get; set; } = Guid.NewGuid();
    public string Name { get; set; } = default!;
    public string Map { get; set; } = default!;
    public string Description { get; set; } = default!;
    public int PlayerCount { get; set; }
    public bool HasPassword { get; set; }
    public string IpAddress { get; set; } = default!;
    public int Port { get; set; }
    public DateTime Created { get; set; }
    public DateTime Updated { get; set; }
    public Guid KeepAliveToken { get; set; } = Guid.NewGuid();


    public string Host => $"{IpAddress}:{Port}";
}