using Microsoft.AspNetCore.Authorization;
using RLLobby.Server.Contracts.Requests;
using RLLobby.Server.Contracts.Responses;
using RLLobby.Server.Data;
using RLLobby.Server.Mappers;

namespace RLLobby.Server.Endpoints;

[HttpGet("lobby/{id}")]
[AllowAnonymous]
public class GetLobbyEndpoint(ILobbyRepository lobbyRepository, ILobbyMapper mapper) : Endpoint<GetLobbyRequest, GetLobbyResponse>
{
    public override async Task HandleAsync(GetLobbyRequest req, CancellationToken ct)
    {
        var lobby = await lobbyRepository.GetLobbyByIdAsync(req.Id);
        if (lobby == null)
        {
            await SendNotFoundAsync(ct);
            return;
        }

        await SendAsync(mapper.MapToGetResponse(lobby), cancellation: ct);
    }
}