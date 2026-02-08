using Microsoft.AspNetCore.Identity;
using Microsoft.EntityFrameworkCore;
using Microsoft.AspNetCore.Authentication;
using Microsoft.AspNetCore.Authentication.Cookies;


var builder = WebApplication.CreateBuilder(args);


builder.Services.AddTransient<ResultsCollection.IResultsService, ResultsCollection.ResultsService>();
builder.Services.AddControllers();
builder.Services.AddEndpointsApiExplorer();
builder.Services.AddSwaggerGen();


builder.Services.AddDbContext<ApplicationDbContext>(options =>
    options.UseInMemoryDatabase("IdentityDb"));


builder.Services.AddIdentity<ApplicationUser, IdentityRole>(options =>
{
    
    options.SignIn.RequireConfirmedAccount = false;
    options.Password.RequiredLength = 4;
    options.Password.RequireDigit = false;
    options.Password.RequireLowercase = false;
    options.Password.RequireUppercase = false;
    options.Password.RequireNonAlphanumeric = false;
})
    .AddEntityFrameworkStores<ApplicationDbContext>()
    .AddDefaultTokenProviders();

// 3. Включение Cookie-аутентификации
builder.Services
    .AddAuthentication(opts =>
    {
        opts.DefaultScheme = CookieAuthenticationDefaults.AuthenticationScheme;
        opts.DefaultAuthenticateScheme = CookieAuthenticationDefaults.AuthenticationScheme;
        opts.DefaultChallengeScheme = CookieAuthenticationDefaults.AuthenticationScheme;
    })
    .AddCookie(o =>
    {
        o.Cookie.Name = ".ASPA0010.Auth";
        o.Cookie.HttpOnly = true;
        o.Cookie.SameSite = SameSiteMode.Lax;       
        o.Cookie.SecurePolicy = CookieSecurePolicy.SameAsRequest;

        
        o.Events.OnRedirectToLogin = ctx =>
        {
            ctx.Response.StatusCode = StatusCodes.Status401Unauthorized;
            return Task.CompletedTask;
        };
        o.Events.OnRedirectToAccessDenied = ctx =>
        {
            ctx.Response.StatusCode = StatusCodes.Status403Forbidden;
            return Task.CompletedTask;
        };
    });


builder.Services.AddControllersWithViews();
builder.Services.AddHttpContextAccessor(); 
builder.Services.AddScoped<ResultsAuthenticate.IAuthenticateService, ResultsAuthenticate.AuthenticateService<ApplicationUser>>();



var app = builder.Build();


if (app.Environment.IsDevelopment())
{
    app.UseSwagger();
    app.UseSwaggerUI();
}


app.UseHttpsRedirection();
app.UseStaticFiles(); 

app.UseRouting(); 
app.UseAuthentication();
app.UseAuthorization();
app.MapControllers();
app.MapControllerRoute(
    name: "default",
    pattern: "{controller=Home}/{action=Index}/{id?}");


async Task InitializeData(IServiceProvider serviceProvider)
{
    var roleManager = serviceProvider.GetRequiredService<RoleManager<IdentityRole>>();
    var userManager = serviceProvider.GetRequiredService<UserManager<ApplicationUser>>();

    string[] roleNames = { "READER", "WRITER" };
    foreach (var roleName in roleNames)
    {
        if (await roleManager.FindByNameAsync(roleName) == null)
            await roleManager.CreateAsync(new IdentityRole(roleName));
    }

    async Task EnsureUser(string login, string password, string role)
    {
        if (await userManager.FindByNameAsync(login) == null)
        {
            var user = new ApplicationUser { UserName = login, Email = $"{login}@test.com" };
            var result = await userManager.CreateAsync(user, password);
            if (result.Succeeded)
                await userManager.AddToRoleAsync(user, role);
        }
    }

    
    await EnsureUser("ReaderUser", "Reader123", "READER");
    await EnsureUser("WriterUser", "Writer123", "WRITER");
    //    {
    //        "login": "ReaderUser",
    //    "password": "Reader123"
    //}

    //    {
    //        "login": "WriterUser",
    //    "password": "Writer123"
    //}
}


using (var scope = app.Services.CreateScope())
{
    await InitializeData(scope.ServiceProvider);
}

app.Run();