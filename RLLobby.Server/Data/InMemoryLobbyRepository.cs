using RLLobby.Server.Domain;

namespace RLLobby.Server.Data;

public class InMemoryLobbyRepository : ILobbyRepository
{
    private readonly Dictionary<Guid, Lobby> _lobbyList = new();

    public Task<Lobby?> CreateAsync(Lobby lobby)
    {
        return !_lobbyList.TryAdd(lobby.Id, lobby) ? Task.FromResult<Lobby?>(null) : Task.FromResult<Lobby?>(lobby);
    }

    public Task<Lobby?> GetLobbyByIdAsync(Guid id)
    {
        return Task.FromResult(_lobbyList.GetValueOrDefault(id));
    }

    public Task<List<Lobby>> GetAllAsync()
    {
        return Task.FromResult(_lobbyList.Select(x => x.Value).ToList());
    }

    public async Task<bool> DeleteLobbyAsync(Guid id, Guid token)
    {
        var lobby = await GetLobbyByIdAndToken(id, token);
        if (lobby == null)
        {
            return false;
        }

        _lobbyList.Remove(lobby.Id);
        return true;
    }

    public async Task<bool> LobbyKeepAliveAsync(Guid id, Guid token)
    {
        var lobby = await GetLobbyByIdAndToken(id, token);
        if (lobby == null)
        {
            return false;
        }

        lobby.Updated = DateTimeOffset.UtcNow;
        _lobbyList[lobby.Id] = lobby;
        return true;
    }

    public async Task<Lobby?> UpdateLobbyAsync(Lobby lobby)
    {
        var realLobby = await GetLobbyByIdAndToken(lobby.Id, lobby.Token);
        if (realLobby is null)
        {
            return null;
        }
        lobby.Updated = DateTimeOffset.UtcNow;
        _lobbyList[realLobby.Id] = lobby;
        return lobby;
    }

    private async Task<Lobby?> GetLobbyByIdAndToken(Guid id, Guid token)
    {
        var lobby = await GetLobbyByIdAsync(id);
        if (lobby is null || lobby.Token != token)
        {
            return null;
        }

        return lobby;
    }
}