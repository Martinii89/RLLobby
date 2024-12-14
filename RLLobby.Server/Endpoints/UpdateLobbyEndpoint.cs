using Microsoft.AspNetCore.Authorization;
using RLLobby.Server.Contracts.Requests;
using RLLobby.Server.Contracts.Responses;
using RLLobby.Server.Data;
using RLLobby.Server.Mappers;

namespace RLLobby.Server.Endpoints;

[HttpPut("lobby/{id}")]
[AllowAnonymous]
public class UpdateLobbyEndpoint(ILobbyRepository lobbyRepository, ILobbyMapper mapper) : Endpoint<UpdateLobbyReqeust, UpdateLobbyResponse>
{
    public override async Task HandleAsync(UpdateLobbyReqeust req, CancellationToken ct)
    {
        var updatedLobby = await lobbyRepository.UpdateLobbyAsync(mapper.MapToLobby(req));
        if (updatedLobby is null)
        {
            ThrowError("Failed to update lobby");
        }

        await SendOkAsync(mapper.MapToUpdateResponse(updatedLobby), ct);
    }
}