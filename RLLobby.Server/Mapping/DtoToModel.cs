using RLLobby.Server.Contracts.Requests;
using RLLobby.Server.Models;

namespace RLLobby.Server.Mapping;

public static class DtoToModel
{
    public static Lobby ToLobby(this CreateLobbyRequest req)
    {
        return new Lobby()
        {
            Id = Guid.NewGuid(),
            KeepAliveToken = Guid.NewGuid(),
            Name = req.Name,
            Map = req.Map,
            PlayerCount = req.PlayerCount,
            Description = req.Description,
            HasPassword = req.HasPassword,
            IpAddress = req.IpAddress!,
            Port = req.Port,
            Created = DateTime.Now,
            Updated = DateTime.Now,
        };
    }
}