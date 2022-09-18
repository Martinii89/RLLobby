using RLLobby.Server.Contracts.Requests;
using RLLobby.Server.Contracts.Responses;
using RLLobby.Server.Domain;

namespace RLLobby.Server.Mappers;

//[Mapper]
public interface ILobbyMapper
{
    CreateLobbyResponse MapToCreateResponse(Lobby lobby);
    GetLobbyResponse MapToGetResponse(Lobby lobby);
    UpdateLobbyResponse MapToUpdateResponse(Lobby lobby);

    Lobby MapToLobby(CreateLobbyRequest createLobbyRequest);
    Lobby MapToLobby(UpdateLobbyReqeust updateLobbyReqeust);
}