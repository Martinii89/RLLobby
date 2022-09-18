﻿using Mapster;
using RLLobby.Server.Contracts.Requests;
using RLLobby.Server.Contracts.Responses;
using RLLobby.Server.Domain;

namespace RLLobby.Server.Mappers;

public class LobbyMapper : ILobbyMapper
{
    public CreateLobbyResponse MapToCreateResponse(Lobby lobby)
    {
        return lobby.Adapt<CreateLobbyResponse>();
    }

    public GetLobbyResponse MapToGetResponse(Lobby lobby)
    {
        return lobby.Adapt<GetLobbyResponse>();
    }

    public UpdateLobbyResponse MapToUpdateResponse(Lobby lobby)
    {
        return lobby.Adapt<UpdateLobbyResponse>();
    }

    public Lobby MapToLobby(CreateLobbyRequest createLobbyRequest)
    {
        return createLobbyRequest.Adapt<Lobby>();
    }

    public Lobby MapToLobby(UpdateLobbyReqeust updateLobbyReqeust)
    {
        return updateLobbyReqeust.Adapt<Lobby>();
    }
}