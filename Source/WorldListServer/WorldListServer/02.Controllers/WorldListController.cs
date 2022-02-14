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

        [HttpGet]
        public IEnumerable<WorldListInfo> Get()
        {
            List<WorldListInfo> worldInfos = _DB.GetWorldList();
            return worldInfos.ToArray();
        }
    }
}
