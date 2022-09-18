using Microsoft.AspNetCore.Authorization;
using RLLobby.Server.Contracts.Requests;
using RLLobby.Server.Data;

namespace RLLobby.Server.Endpoints;

[HttpDelete("lobby/{id}")]
[AllowAnonymous]
public class DeleteLobbyEndpoint : Endpoint<DeleteLobbyRequest>
{
    private readonly ILobbyRepository m_lobbyRepository;

    public DeleteLobbyEndpoint(ILobbyRepository lobbyRepository)
    {
        m_lobbyRepository = lobbyRepository;
    }

    public override async Task HandleAsync(DeleteLobbyRequest req, CancellationToken ct)
    {
        var deleted = await m_lobbyRepository.DeleteLobbyAsync(req.Id, req.Token);
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