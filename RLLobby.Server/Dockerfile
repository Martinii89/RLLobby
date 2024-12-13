FROM mcr.microsoft.com/dotnet/aspnet:9.0 AS base
USER $APP_UID
WORKDIR /app
EXPOSE 8080
EXPOSE 8081

FROM --platform=$BUILDPLATFORM mcr.microsoft.com/dotnet/sdk:9.0 AS build
ARG TARGETARCH
WORKDIR /src
COPY ["RLLobby.Server/RLLobby.Server.csproj", "RLLobby.Server/"]
RUN dotnet restore "RLLobby.Server/RLLobby.Server.csproj" -a $TARGETARCH
COPY . .
WORKDIR "/src/RLLobby.Server"
RUN dotnet build "RLLobby.Server.csproj" -a $TARGETARCH -c Release -o /app/build

FROM build AS publish
RUN dotnet publish "RLLobby.Server.csproj" -a $TARGETARCH -c Release -o /app/publish /p:UseAppHost=false

FROM base AS final
WORKDIR /app
COPY --from=publish /app/publish .
ENTRYPOINT ["dotnet", "RLLobby.Server.dll"]
