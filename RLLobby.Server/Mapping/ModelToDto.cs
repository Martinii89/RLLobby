using System.Security.Cryptography.X509Certificates;
using RLLobby.Server.Contracts.Responses;
using RLLobby.Server.Models;

namespace RLLobby.Server.Mapping;

public static class ModelToDto
{
    public static GetLobbyResponse ToGetLobbyResponse(this Lobby req)
    {
        return new GetLobbyResponse
        {
            Id = req.Id,
            Name = req.Name,
            Map = req.Map,
            PlayerCount = req.PlayerCount,
            Description = req.Description,
            HasPassword = req.HasPassword,
            IpAddress = req.IpAddress!,
            Port = req.Port,
            Created = req.Created,
            Updated = req.Updated,
        };
    }
}