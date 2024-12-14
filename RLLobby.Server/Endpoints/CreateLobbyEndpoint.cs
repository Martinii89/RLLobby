using RLLobby.Server.Contracts.Requests;
using RLLobby.Server.Contracts.Responses;
using RLLobby.Server.Data;
using RLLobby.Server.Mappers;

namespace RLLobby.Server.Endpoints;

public class CreateLobbyEndpoint(ILobbyRepository lobbyRepository, ILobbyMapper mapper) : Endpoint<CreateLobbyRequest, CreateLobbyResponse>
{
    public override void Configure()
    {
        Post("lobby");
        AllowAnonymous();
        Summary(s =>
        {
            s.RequestParam(r => r.IpAddress, "If this parameter is missing, the server will use the ip of the request");
        });
    }

    public override async Task HandleAsync(CreateLobbyRequest req, CancellationToken ct)
    {
        if (string.IsNullOrEmpty(req.IpAddress))
        {
            req.IpAddress = HttpContext.Connection.RemoteIpAddress?.MapToIPv4().ToString();
        }
        ArgumentNullException.ThrowIfNull(req.IpAddress);

        var lobby = await lobbyRepository.CreateAsync(mapper.MapToLobby(req));
        if (lobby is null)
        {
            ThrowError("Failed to create lobby");
        }

        await SendCreatedAtAsync<GetLobbyEndpoint>(new {id = lobby.Id}, mapper.MapToCreateResponse(lobby), cancellation: ct);
    }
}