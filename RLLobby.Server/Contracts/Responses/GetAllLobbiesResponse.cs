namespace RLLobby.Server.Contracts.Responses;

[PublicAPI]
public class GetAllLobbiesResponse
{
    public List<GetLobbyResponse> Lobbies { get; set; } = [];
}