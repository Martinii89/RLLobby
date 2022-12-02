using System.Net;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.SignalR;
using RLLobby.Server.Contracts.Requests;
using RLLobby.Server.Contracts.Responses;
using RLLobby.Server.Data;
using RLLobby.Server.Mappers;
using RLLobby.Server.SignalR;

namespace RLLobby.Server.Endpoints;

public class CreateLobbyEndpoint : Endpoint<CreateLobbyRequest, CreateLobbyResponse>
{
    private readonly ILobbyRepository m_lobbyRepository;
    private readonly ILobbyMapper m_mapper;
    //private readonly IHubContext<LobbyHub, ILobbyHubClient> m_hubContext;

    public override void Configure()
    {
        Post("lobby");
        AllowAnonymous();
        Summary(s =>
        {
            s.RequestParam(r => r.IpAddress, "If this parameter is missing, the server will use the ip of the request");
        });
    }

    public CreateLobbyEndpoint(ILobbyRepository lobbyRepository, ILobbyMapper mapper/*, IHubContext<LobbyHub, ILobbyHubClient> hubContext*/)
    {
        m_lobbyRepository = lobbyRepository;
        m_mapper = mapper;
        //m_hubContext = hubContext;
    }

    public override async Task HandleAsync(CreateLobbyRequest req, CancellationToken ct)
    {
        if (string.IsNullOrEmpty(req.IpAddress))
        {
            req.IpAddress = HttpContext.Connection.RemoteIpAddress?.MapToIPv4().ToString();
        }
        ArgumentNullException.ThrowIfNull(req.IpAddress);
        //if (!IPAddress.TryParse(req.IpAddress, out _))
        //{
        //    ThrowError("Bad ipaddress");
        //}


        var lobby = await m_lobbyRepository.CreateAsync(m_mapper.MapToLobby(req));
        if (lobby is null)
        {
            ThrowError("Failed to create lobby");
        }

        await SendCreatedAtAsync<GetLobbyEndpoint>(new {id = lobby.Id}, m_mapper.MapToCreateResponse(lobby), cancellation: ct);
        //await m_hubContext.Clients.All.LobbyCreated(m_mapper.MapToGetResponse(lobby));
    }
}