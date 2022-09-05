using RLLobby.Server.Models;

namespace RLLobby.Server.Data;

public interface ILobbyRepository
{
    Task<Lobby?> CreateAsync(Lobby lobby);
    Task<Lobby?> GetLobbyByIdAsync(Guid id);
}

public class InMemoryLobbyRepository : ILobbyRepository
{
    private readonly Dictionary<Guid, Lobby> _lobbyList = new();

    public Task<Lobby?> CreateAsync(Lobby lobby)
    {
        if (_lobbyList.ContainsKey(lobby.Id))
        {
            return Task.FromResult<Lobby?>(null);
        }

        _lobbyList [lobby.Id] = lobby;
        return Task.FromResult<Lobby?>(lobby);
    }

    public Task<Lobby?> GetLobbyByIdAsync(Guid id)
    {
        return Task.FromResult(_lobbyList.Where(lobby => lobby.Value.Id == id).Select(lobby => lobby.Value).FirstOrDefault());
    }
}