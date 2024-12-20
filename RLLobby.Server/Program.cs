using Microsoft.AspNetCore.HttpOverrides;
using FastEndpoints.Swagger;
using RLLobby.Server.Converters;
using RLLobby.Server.Data;
using RLLobby.Server.Mappers;

var builder = WebApplication.CreateBuilder(args);

builder.Services.AddFastEndpoints();
builder.Services.SwaggerDocument(o =>
{
    o.DocumentSettings = s =>
    {
        s.Title = "RLLobby api";
        s.Version = "v1";
    };
    o.ShortSchemaNames = true;
    o.EnableJWTBearerAuth = false;
    o.RemoveEmptyRequestSchema = true;
});


builder.Services.AddSingleton<ILobbyRepository, InMemoryLobbyRepository>();
builder.Services.AddSingleton<ILobbyMapper, LobbyMapper>();
builder.Services.AddHostedService<RemoveOldLobbiesService>();

var app = builder.Build();

app.UseForwardedHeaders(new ForwardedHeadersOptions
{
    ForwardedHeaders = ForwardedHeaders.XForwardedFor | ForwardedHeaders.XForwardedProto
});

app.UseFastEndpoints(c =>
{
    c.Serializer.Options.Converters.Add(new GuidConverter());
});

app.UseOpenApi();
app.UseSwaggerGen();
app.Run();