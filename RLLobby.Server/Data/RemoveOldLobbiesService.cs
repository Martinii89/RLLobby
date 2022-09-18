namespace RLLobby.Server.Data;

public class RemoveOldLobbiesService : BackgroundService
{
    private readonly ILogger<RemoveOldLobbiesService> m_logger;
    private readonly ILobbyRepository m_lobbyRepository;
    private const int CheckIntervalSeconds = 60;
    private const int OldThresholdMinutes = 10;

    public RemoveOldLobbiesService(ILobbyRepository mLobbyRepository, ILogger<RemoveOldLobbiesService> mLogger)
    {
        m_lobbyRepository = mLobbyRepository;
        m_logger = mLogger;
    }


    protected override async Task ExecuteAsync(CancellationToken stoppingToken)
    {
        while (!stoppingToken.IsCancellationRequested)
        {
            var allLobies = await m_lobbyRepository.GetAllAsync();
            var oldLobbies = allLobies.Where(x => x.Updated.AddMinutes(OldThresholdMinutes) < DateTime.Now);
            foreach (var oldLobby in oldLobbies)
            {
                m_logger.LogDebug("Removing lobby {} for inactivity", oldLobby.Name);
                await m_lobbyRepository.DeleteLobbyAsync(oldLobby.Id, oldLobby.Token);
            }

            await Task.Delay(TimeSpan.FromSeconds(CheckIntervalSeconds), stoppingToken);
        }
    }
}