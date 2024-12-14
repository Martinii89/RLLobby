
namespace RLLobby.Server.Contracts.Requests;

[PublicAPI]
public class DeleteLobbyRequest
{
    public Guid Id { get; set; }
    [FromHeader]
    public Guid Token { get; set; }
}