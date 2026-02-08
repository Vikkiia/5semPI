using System.Security.Claims;
using System.Threading.Tasks;

namespace ResultsAuthenticate
{
    public interface IAuthenticateService
    {
        Task<ClaimsPrincipal?> SignInAsync(string login, string password);
        Task SignOutAsync();
    }
}
