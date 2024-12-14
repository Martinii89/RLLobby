using Microsoft.AspNetCore.Authorization;
using RLLobby.Server.Contracts.Requests;
using RLLobby.Server.Data;

namespace RLLobby.Server.Endpoints;

[HttpDelete("lobby/{id}")]
[AllowAnonymous]
public class DeleteLobbyEndpoint(ILobbyRepository lobbyRepository) : Endpoint<DeleteLobbyRequest>
{
    public override async Task HandleAsync(DeleteLobbyRequest req, CancellationToken ct)
    {
        var deleted = await lobbyRepository.DeleteLobbyAsync(req.Id, req.Token);
        if (deleted)
        {
            await SendNoContentAsync(ct);
        }
        else
        {
            await SendNotFoundAsync(ct);
        }
    }
}