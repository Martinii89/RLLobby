using Microsoft.AspNetCore.Authorization;
using RLLobby.Server.Contracts.Responses;
using RLLobby.Server.Data;
using RLLobby.Server.Mappers;

namespace RLLobby.Server.Endpoints;

[HttpGet("lobby")]
[AllowAnonymous]
public class GetAllLobbiesEndpoint(ILobbyRepository lobbyRepository, ILobbyMapper mapper) : EndpointWithoutRequest<GetAllLobbiesResponse>
{
    public override async Task HandleAsync(CancellationToken ct)
    {
        var lobbies = await lobbyRepository.GetAllAsync();
        var response = lobbies.Select(x => mapper.MapToGetResponse(x)).ToList();
        await SendOkAsync(new GetAllLobbiesResponse{Lobbies = response}, ct);
    }
}