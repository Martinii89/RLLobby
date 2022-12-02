using System.Runtime.Serialization;
using Microsoft.AspNetCore.HttpOverrides;
using FastEndpoints.Swagger;
using RLLobby.Server.Converters;
using RLLobby.Server.Data;
using RLLobby.Server.Mappers;
using RLLobby.Server.SignalR;

var builder = WebApplication.CreateBuilder(args);

//builder.Services.AddCors();
//builder.Services.AddSignalR();
builder.Services.AddFastEndpoints();
builder.Services.AddSwaggerDoc(o =>
{
    o.Title = "RLLobby api";
    o.Version = "v1";
}, shortSchemaNames: true, addJWTBearerAuth: false, removeEmptySchemas:true);

builder.Services.AddSingleton<ILobbyRepository, InMemoryLobbyRepository>();
builder.Services.AddSingleton<ILobbyMapper, LobbyMapper>();
builder.Services.AddHostedService<RemoveOldLobbiesService>();

var app = builder.Build();

//app.UseCors(x => x
//    .AllowAnyMethod()
//    .AllowAnyHeader()
//    .SetIsOriginAllowed(origin => true) // allow any origin
//    .AllowCredentials()); // allow credentials

app.UseForwardedHeaders(new ForwardedHeadersOptions
{
    ForwardedHeaders = ForwardedHeaders.XForwardedFor | ForwardedHeaders.XForwardedProto
});

app.UseFastEndpoints(c =>
{
    c.Serializer.Options.Converters.Add(new GuidConverter());
});
//app.MapHub<LobbyHub>("/signalr/lobby");


app.UseOpenApi();
app.UseSwaggerUi3(s => s.ConfigureDefaults());


app.Run();