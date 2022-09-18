using RLLobby.Server.Contracts.Requests;

namespace RLLobby.Server.Validation;

public class LobbyKeepAliveValidator : Validator<LobbyKeepAliveRequest>
{
    public LobbyKeepAliveValidator()
    {
        RuleFor(x => x.Id).NotEmpty();
        RuleFor(x => x.Token).NotEmpty();
    }
}