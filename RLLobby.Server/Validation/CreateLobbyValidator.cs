using RLLobby.Server.Contracts.Requests;

namespace RLLobby.Server.Validation;

public class CreateLobbyValidator: Validator<CreateLobbyRequest>
{
    public CreateLobbyValidator()
    {
        RuleFor(l => l.Name).NotEmpty();
        RuleFor(l => l.Map).NotEmpty();
        RuleFor(l => l.PlayerCount).GreaterThanOrEqualTo(0);
        RuleFor(l => l.Port).NotNull();
    }
}