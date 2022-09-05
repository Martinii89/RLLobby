using FastEndpoints;
using Microsoft.AspNetCore.Authorization;
using RLLobby.Server.Contracts.Requests;
using RLLobby.Server.Contracts.Responses;
using RLLobby.Server.Data;
using RLLobby.Server.Mapping;

namespace RLLobby.Server.Endpoints;

[HttpGet("lobby/{id}")]
[AllowAnonymous]
public class GetLobbyEndpoint : Endpoint<GetLobbyRequest, GetLobbyResponse>
{
    private readonly ILobbyRepository _lobbyRepository;

    public GetLobbyEndpoint(ILobbyRepository lobbyRepository)
    {
        _lobbyRepository = lobbyRepository;
    }

    public override async Task HandleAsync(GetLobbyRequest req, CancellationToken ct)
    {
        var lobby = await _lobbyRepository.GetLobbyByIdAsync(req.Id);
        if (lobby == null)
        {
            await SendNotFoundAsync(ct);
            return;
        }

        await SendAsync(lobby.ToGetLobbyResponse(), cancellation: ct);
    }
}