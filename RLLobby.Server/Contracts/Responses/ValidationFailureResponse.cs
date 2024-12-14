namespace RLLobby.Server.Contracts.Responses;

[PublicAPI]
public class ValidationFailureResponse
{
    public List<string> Errors { get; init; } = [];
}