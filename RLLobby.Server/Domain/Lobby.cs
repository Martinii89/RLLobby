namespace RLLobby.Server.Domain;

public class Lobby
{
    public Guid Id { get; set; } = Guid.NewGuid();
    public string Name { get; set; } = default!;
    public string Map { get; set; } = default!;
    public string Description { get; set; } = default!;
    public int PlayerCount { get; set; }
    public List<string> Players { get; set; } = new();
    public bool HasPassword { get; set; }
    public string IpAddress { get; set; } = default!;
    public int Port { get; set; }
    public DateTime Created { get; set; } = DateTime.Now;
    public DateTime Updated { get; set; } = DateTime.Now;
    public Guid Token { get; set; } = Guid.NewGuid();


    public string Host => $"{IpAddress}:{Port}";
}