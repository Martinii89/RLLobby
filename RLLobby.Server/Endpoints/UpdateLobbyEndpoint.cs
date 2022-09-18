using Microsoft.AspNetCore.Authorization;
using RLLobby.Server.Contracts.Requests;
using RLLobby.Server.Contracts.Responses;
using RLLobby.Server.Data;
using RLLobby.Server.Domain;
using RLLobby.Server.Mappers;

namespace RLLobby.Server.Endpoints;

[HttpPut("lobby/{id}")]
[AllowAnonymous]
public class UpdateLobbyEndpoint : Endpoint<UpdateLobbyReqeust, UpdateLobbyResponse>
{
    private readonly ILobbyRepository m_lobbyRepository;
    private readonly ILobbyMapper m_mapper;

    public UpdateLobbyEndpoint(ILobbyRepository lobbyRepository, ILobbyMapper mapper)
    {
        m_lobbyRepository = lobbyRepository;
        m_mapper = mapper;
    }

    public override async Task HandleAsync(UpdateLobbyReqeust req, CancellationToken ct)
    {
        var updatedLobby = await m_lobbyRepository.UpdateLobbyAsync(m_mapper.MapToLobby(req));
        if (updatedLobby is null)
        {
            ThrowError("Failed to update lobby");
        }

        await SendOkAsync(m_mapper.MapToUpdateResponse(updatedLobby), ct);
    }
}