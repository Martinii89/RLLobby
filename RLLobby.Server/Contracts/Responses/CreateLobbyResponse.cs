namespace RLLobby.Server.Contracts.Responses;

public class CreateLobbyResponse
{
    public Guid LobbyToken { get; set; } = default!;
}