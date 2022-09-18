using RLLobby.Server.Contracts.Requests;

namespace RLLobby.Server.Validation;

public class UpdateLobbyValidator : Validator<UpdateLobbyReqeust>
{
    public UpdateLobbyValidator()
    {
        //id and token validation
        RuleFor(x => x.Id).NotEmpty();
        RuleFor(x => x.Token).NotEmpty();

        // lobby params validation
        RuleFor(l => l.Name).NotEmpty();
        RuleFor(l => l.Map).NotEmpty();
        RuleFor(l => l.PlayerCount).GreaterThanOrEqualTo(0);
        RuleFor(l => l.Port).NotNull();
    }
}