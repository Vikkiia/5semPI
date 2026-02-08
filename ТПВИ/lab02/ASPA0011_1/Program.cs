using ASPA0011_1.Services;
using ASPA0011_1.Models;
using Serilog;

var builder = WebApplication.CreateBuilder(args);

// Настройка Serilog
Log.Logger = new LoggerConfiguration()
    .MinimumLevel.Verbose() // Вместо Trace используем Verbose, он типо самый низкий уровень который соответсвует Verbose
    .MinimumLevel.Override("Microsoft", Serilog.Events.LogEventLevel.Warning) // Поонижаем
    .Enrich.FromLogContext()
    .WriteTo.Console(restrictedToMinimumLevel: Serilog.Events.LogEventLevel.Warning)
    .WriteTo.File("logs/app-.txt",
        rollingInterval: RollingInterval.Day,
        restrictedToMinimumLevel: Serilog.Events.LogEventLevel.Verbose)
    .WriteTo.Debug(restrictedToMinimumLevel: Serilog.Events.LogEventLevel.Debug)
    .CreateLogger();

builder.Host.UseSerilog();

builder.Services.AddControllers();
builder.Services.AddEndpointsApiExplorer();
builder.Services.AddSwaggerGen();

// Регистрация сервисов
builder.Services.AddSingleton<IChannelService, ChannelService>();

// Настройка конфигурации
builder.Services.Configure<AppSettings>(builder.Configuration.GetSection("AppSettings"));

var app = builder.Build();

// Запросы в режиме development
if (app.Environment.IsDevelopment())
{
    app.UseSwagger();
    app.UseSwaggerUI();
}

app.UseHttpsRedirection();
app.UseAuthorization();
app.MapControllers();

Log.Information("Application starting up");

try
{
    app.Run();
}
catch (Exception ex)
{
    Log.Fatal(ex, "Application terminated unexpectedly");
}
finally
{
    Log.CloseAndFlush();
}