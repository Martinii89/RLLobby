using RLLobby.Server.Domain;

namespace RLLobby.Server.Data;

public interface ILobbyRepository
{
    Task<Lobby?> CreateAsync(Lobby lobby);
    Task<Lobby?> GetLobbyByIdAsync(Guid id);
    Task<List<Lobby>> GetAllAsync();
    Task<bool> DeleteLobbyAsync(Guid id, Guid token);
    Task<bool> LobbyKeepAliveAsync(Guid id, Guid token);
    Task<Lobby?> UpdateLobbyAsync(Lobby lobby);
}