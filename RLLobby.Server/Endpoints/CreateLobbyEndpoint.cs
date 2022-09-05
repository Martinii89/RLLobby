using System.Net;
using FastEndpoints;
using Microsoft.AspNetCore.Authorization;
using RLLobby.Server.Contracts.Requests;
using RLLobby.Server.Contracts.Responses;
using RLLobby.Server.Data;
using RLLobby.Server.Mapping;

namespace RLLobby.Server.Endpoints;

[HttpPost("lobby")]
[AllowAnonymous]
public class CreateLobbyEndpoint : Endpoint<CreateLobbyRequest, CreateLobbyResponse>
{
    private readonly ILobbyRepository _lobbyRepository;

    public CreateLobbyEndpoint(ILobbyRepository lobbyRepository)
    {
        _lobbyRepository = lobbyRepository;
    }

    public override async Task HandleAsync(CreateLobbyRequest req, CancellationToken ct)
    {
        req.IpAddress ??= HttpContext.Connection.RemoteIpAddress?.MapToIPv4().ToString();
        ArgumentNullException.ThrowIfNull(req.IpAddress);
        if (!IPAddress.TryParse(req.IpAddress, out _))
        {
            ThrowError("Bad ipaddress");
        }


        var lobby = await _lobbyRepository.CreateAsync(req.ToLobby());
        if (lobby is null)
        {
            ThrowError("Failed to create lobby");
        }

        await SendCreatedAtAsync<GetLobbyEndpoint>(new {id = lobby.Id}, new CreateLobbyResponse { LobbyToken = lobby.KeepAliveToken }, cancellation: ct);
    }
}