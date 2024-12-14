namespace RLLobby.Server.Data;

public class RemoveOldLobbiesService(ILobbyRepository mLobbyRepository, ILogger<RemoveOldLobbiesService> mLogger) : BackgroundService
{
    private const int CheckIntervalSeconds = 30;
    private const int OldThresholdMinutes = 2;


    protected override async Task ExecuteAsync(CancellationToken stoppingToken)
    {
        while (!stoppingToken.IsCancellationRequested)
        {
            var allLobies = await mLobbyRepository.GetAllAsync();
            var oldLobbies = allLobies.Where(x => x.Updated.AddMinutes(OldThresholdMinutes) < DateTimeOffset.UtcNow);
            foreach (var oldLobby in oldLobbies)
            {
                mLogger.LogDebug("Removing lobby {LobbyName} for inactivity", oldLobby.Name);
                await mLobbyRepository.DeleteLobbyAsync(oldLobby.Id, oldLobby.Token);
            }

            await Task.Delay(TimeSpan.FromSeconds(CheckIntervalSeconds), stoppingToken);
        }
    }
}