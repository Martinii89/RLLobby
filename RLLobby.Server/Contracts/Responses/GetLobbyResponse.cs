namespace RLLobby.Server.Contracts.Responses;

/// <summary>
/// Includes all the fields of <see cref="RLLobby.Server.Models.Lobby"/> except the KeepAlive token
/// </summary>
public class GetLobbyResponse
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
}