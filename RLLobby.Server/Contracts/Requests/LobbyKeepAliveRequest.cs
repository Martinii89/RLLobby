namespace RLLobby.Server.Contracts.Requests;

[PublicAPI]
public class LobbyKeepAliveRequest
{
    public Guid Id { get; set; }
    [FromHeader]
    public Guid Token { get; set; }
}