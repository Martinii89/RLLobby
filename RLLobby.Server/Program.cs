using Microsoft.AspNetCore.HttpOverrides;
using FastEndpoints;
using FastEndpoints.Swagger;
using RLLobby.Server.Contracts.Responses;
using RLLobby.Server.Data;
using RLLobby.Server.Validation;

var builder = WebApplication.CreateBuilder(args);


builder.Services.AddFastEndpoints();
builder.Services.AddSwaggerDoc();

builder.Services.AddSingleton<ILobbyRepository, InMemoryLobbyRepository>();

var app = builder.Build();

app.UseForwardedHeaders(new ForwardedHeadersOptions
{
    ForwardedHeaders = ForwardedHeaders.XForwardedFor | ForwardedHeaders.XForwardedProto
});

app.UseMiddleware<ValidationExceptionMiddleware>();
app.UseFastEndpoints(x =>
{
    x.Errors.ResponseBuilder = (failures, _) =>
    {
        return new ValidationFailureResponse
        {
            Errors = failures.Select(y => y.ErrorMessage).ToList()
        };
    };
});


app.UseOpenApi();
app.UseSwaggerUi3(s => s.ConfigureDefaults());


app.Run();