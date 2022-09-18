namespace RLLobby.Server.Contracts.Requests;

public class LobbyKeepAliveRequest
{
    public Guid Id { get; set; }
    [FromHeader]
    public Guid Token { get; set; }
}