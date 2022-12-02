using Microsoft.AspNetCore.SignalR;
using RLLobby.Server.Contracts.Responses;

namespace RLLobby.Server.SignalR;

public interface ILobbyHubClient
{
    Task LobbyCreated(GetLobbyResponse response);
}

public class LobbyHub : Hub<ILobbyHubClient>
{
    public async Task SendLobbyCreated(string user, GetLobbyResponse response)
    {
        await Clients.All.LobbyCreated(response);
    }
}