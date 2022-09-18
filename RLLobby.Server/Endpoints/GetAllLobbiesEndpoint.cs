using Microsoft.AspNetCore.Authorization;
using RLLobby.Server.Contracts.Responses;
using RLLobby.Server.Data;
using RLLobby.Server.Mappers;

namespace RLLobby.Server.Endpoints;

[HttpGet("lobby")]
[AllowAnonymous]
public class GetAllLobbiesEndpoint : EndpointWithoutRequest<GetAllLobbiesResponse>
{
    private readonly ILobbyRepository m_lobbyRepository;
    private readonly ILobbyMapper m_mapper;

    public GetAllLobbiesEndpoint(ILobbyRepository lobbyRepository, ILobbyMapper mapper)
    {
        m_lobbyRepository = lobbyRepository;
        m_mapper = mapper;
    }

    public override async Task HandleAsync(CancellationToken ct)
    {
        var lobbies = await m_lobbyRepository.GetAllAsync();
        var response = lobbies.Select(x => m_mapper.MapToGetResponse(x)).ToList();
        await SendOkAsync(new GetAllLobbiesResponse{Lobbies = response}, ct);
    }
}