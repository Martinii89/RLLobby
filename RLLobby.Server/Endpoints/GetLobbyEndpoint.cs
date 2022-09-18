using Microsoft.AspNetCore.Authorization;
using RLLobby.Server.Contracts.Requests;
using RLLobby.Server.Contracts.Responses;
using RLLobby.Server.Data;
using RLLobby.Server.Mappers;

namespace RLLobby.Server.Endpoints;

[HttpGet("lobby/{id}")]
[AllowAnonymous]
public class GetLobbyEndpoint : Endpoint<GetLobbyRequest, GetLobbyResponse>
{
    private readonly ILobbyRepository m_lobbyRepository;
    private readonly ILobbyMapper m_mapper;

    public GetLobbyEndpoint(ILobbyRepository lobbyRepository,  ILobbyMapper mapper)
    {
        m_lobbyRepository = lobbyRepository;
        m_mapper = mapper;
    }

    public override async Task HandleAsync(GetLobbyRequest req, CancellationToken ct)
    {
        var lobby = await m_lobbyRepository.GetLobbyByIdAsync(req.Id);
        if (lobby == null)
        {
            await SendNotFoundAsync(ct);
            return;
        }

        await SendAsync(m_mapper.MapToGetResponse(lobby), cancellation: ct);
    }
}