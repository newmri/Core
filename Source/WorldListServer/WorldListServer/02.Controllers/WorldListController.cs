using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace WorldListServer.Controllers
{
    [ApiController]
    [Route("[controller]")]
    public class WorldListController : ControllerBase
    {
        private DB _DB = new DB();
        private readonly ILogger<WorldListController> _logger;

        public WorldListController(ILogger<WorldListController> logger)
        {
            _logger = logger;
        }

        [HttpPost]
		[Route("signup")]
		public SignupAccountPacketRes SignupAccount([FromBody] SignupAccountPacketReq req)
		{
            SignupAccountPacketRes res = new SignupAccountPacketRes();

            res.IsSuccess = _DB.SignupAccount(req);

            return res;
		}

        [HttpPost]
        [Route("login")]
        public LoginAccountPacketRes LoginAccount([FromBody] LoginAccountPacketReq req)
        {
            LoginAccountPacketRes res;

            res = _DB.LoginAccount(req);

            return res;
        }

        [HttpPost]
        [Route("serverselect")]
        public LoginServerInfoPacketRes ServerSelect([FromBody] LoginServerInfoPacketReq req)
        {
            LoginServerInfoPacketRes res;

            res = _DB.GetMinServerInfo(req);

            return res;
        }

        [HttpGet]
        public IEnumerable<WorldListInfo> Get()
        {
            List<WorldListInfo> worldInfos = _DB.GetWorldList();
            return worldInfos.ToArray();
        }
    }
}
