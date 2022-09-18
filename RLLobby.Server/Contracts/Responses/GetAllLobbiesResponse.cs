using RLLobby.Server.Contracts.Requests;

namespace RLLobby.Server.Contracts.Responses;

public class GetAllLobbiesResponse
{
    public List<GetLobbyResponse> Lobbies { get; set; } = new();
}