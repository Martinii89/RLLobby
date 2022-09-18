using RLLobby.Server.Contracts.Requests;

namespace RLLobby.Server.Validation;

public class DeleteLobbyValidator: Validator<DeleteLobbyRequest>
{
    public DeleteLobbyValidator()
    {
        RuleFor(x => x.Id).NotEmpty();
        RuleFor(x => x.Token).NotEmpty();
    }
}