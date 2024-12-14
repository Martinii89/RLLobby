using Microsoft.AspNetCore.Authorization;
using RLLobby.Server.Contracts.Requests;
using RLLobby.Server.Contracts.Responses;
using RLLobby.Server.Data;

namespace RLLobby.Server.Endpoints;

[FastEndpoints.HttpPost("lobby/{id}/alive")]
[AllowAnonymous]
public class KeepAliveLobbyEndpoint(ILobbyRepository lobbyRepository) : Endpoint<LobbyKeepAliveRequest, LobbyKeepAliveResponse>
{
    public override async Task HandleAsync(LobbyKeepAliveRequest req, CancellationToken ct)
    {
        var result = await lobbyRepository.LobbyKeepAliveAsync(req.Id, req.Token);
        if (!result)
        {
            ThrowError("Failed to keep lobby alive");
        }

        await SendOkAsync(new LobbyKeepAliveResponse{Result = true}, ct);
    }
}