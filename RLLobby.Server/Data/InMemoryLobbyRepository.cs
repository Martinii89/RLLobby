using RLLobby.Server.Domain;
using YamlDotNet.Core.Tokens;

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

public class InMemoryLobbyRepository : ILobbyRepository
{
    private readonly Dictionary<Guid, Lobby> m_lobbyList = new();

    public Task<Lobby?> CreateAsync(Lobby lobby)
    {
        if (m_lobbyList.ContainsKey(lobby.Id))
        {
            return Task.FromResult<Lobby?>(null);
        }

        m_lobbyList[lobby.Id] = lobby;
        return Task.FromResult<Lobby?>(lobby);
    }

    public Task<Lobby?> GetLobbyByIdAsync(Guid id)
    {
        return Task.FromResult(m_lobbyList.GetValueOrDefault(id));
    }

    public Task<List<Lobby>> GetAllAsync()
    {
        return Task.FromResult(m_lobbyList.Select(x => x.Value).ToList());
    }

    public async Task<bool> DeleteLobbyAsync(Guid id, Guid token)
    {
        var lobby = await GetLobbyByIdAndToken(id, token);
        if (lobby == null)
        {
            return false;
        }

        m_lobbyList.Remove(lobby.Id);
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
        m_lobbyList[lobby.Id] = lobby;
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
        m_lobbyList[realLobby.Id] = lobby;
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