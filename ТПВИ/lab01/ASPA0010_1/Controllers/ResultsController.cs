using Microsoft.AspNetCore.Mvc;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Authentication;
using Microsoft.AspNetCore.Authentication.Cookies;
using ResultsCollection;
using ResultsAuthenticate;


[Route("api/[controller]")]
[ApiController]
public class ResultsController : ControllerBase
{
    
    public class ResultValueModel { public string Value { get; set; } }


    public class SignInModel { 
        public string Login { get; set; } 
        public string Password { get; set; } 
    }

    private readonly IResultsService _service;
    private readonly IAuthenticateService _authService; 

    public ResultsController(IResultsService service, IAuthenticateService authService)
    {
        _service = service;
        _authService = authService;
    }



    // POST /api/Results/SignIn
    [HttpPost("SignIn")]
    [AllowAnonymous]
    public async Task<IActionResult> SignIn([FromBody] SignInModel model)
    {
        if (string.IsNullOrEmpty(model?.Login) || string.IsNullOrEmpty(model?.Password))
            return BadRequest(); // 400 Bad Request 

        var principal = await _authService.SignInAsync(model.Login, model.Password);

        if (principal == null)
            return NotFound(); // 404 Not Found (Пользователь не найден или пароль неверен) 

        
        await HttpContext.SignInAsync(CookieAuthenticationDefaults.AuthenticationScheme,
                                     principal,
                                     new AuthenticationProperties { IsPersistent = true });

        return Ok(new { Message = "Authentication successful" }); // 200 OK 
    }

    //    {
    //        "login": "ReaderUser",
    //    "password": "Reader123"
    //}

    //    {
    //        "login": "WriterUser",
    //    "password": "Writer123"
    //}

    // GET /api/Results/SignOut
    [HttpGet("SignOut")]

    [Authorize] 
    public async Task<IActionResult> SignOut()
    {
        await HttpContext.SignOutAsync(CookieAuthenticationDefaults.AuthenticationScheme);
        await _authService.SignOutAsync();
        return Ok(new { Message = "Sign out successful" }); // 200 OK 
    }




    // GET /api/Results
    [HttpGet]

    [Authorize(Roles = "READER,WRITER")]
    public async Task<ActionResult<IEnumerable<ResultItem>>> GetAll()
    {
        var results = await _service.GetAllAsync();
        if (results == null || !results.Any())
            return NoContent(); // 204 No Content
        return Ok(results); // 200 OK
        // 401 Unauthorized (будет возвращаться автоматически при отсутствии Cookie)
    }

    // GET /api/Results/{k:int}
    [HttpGet("{k:int}")]
    [Authorize(Roles = "READER,WRITER")]
    public async Task<ActionResult<ResultItem>> Get(int k)
    {
        var result = await _service.GetAsync(k);
        if (result == null)
            return NotFound(); // 404 Not Found
        return Ok(result); // 200 OK
    }

    // POST /api/Results/
    [HttpPost]
    [Authorize(Roles = "WRITER")]
    public async Task<ActionResult<ResultItem>> Add([FromBody] ResultValueModel model)
    {
        if (model == null || string.IsNullOrEmpty(model.Value))
            return BadRequest(); // 400 Bad Request

        var newResult = await _service.AddAsync(model.Value);
        return CreatedAtAction(nameof(Get), new { k = newResult.Key }, newResult); // 201 Created
    }

    // PUT /api/Results/{k:int}
    [HttpPut("{k:int}")]
    [Authorize(Roles = "WRITER")]
    public async Task<ActionResult<ResultItem>> Update(int k, [FromBody] ResultValueModel model)
    {
        if (model == null || string.IsNullOrEmpty(model.Value))
            return BadRequest(); // 400 Bad Request

        var updatedResult = await _service.UpdateAsync(k, model.Value);
        if (updatedResult == null)
            return NotFound(); // 404 Not Found
        return Ok(updatedResult); // 200 OK
    }

    // DELETE /api/Results/{k:int}
    [HttpDelete("{k:int}")]
    [Authorize(Roles = "WRITER")]
    public async Task<ActionResult<ResultItem>> Delete(int k)
    {
        var deletedResult = await _service.DeleteAsync(k);
        if (deletedResult == null)
            return NotFound(); // 404 Not Found
        return Ok(deletedResult); // 200 OK
    }
}