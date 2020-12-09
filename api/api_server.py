from aiohttp import web
import json
from sms import publish

with open('config.json', 'r') as config_file:
    config = json.load(config_file)


# define endpoints
async def low_alert(request):
    response = publish()
    return web.json_response(data=response)


# initialize application
app = web.Application()

app.add_routes(
    [
        web.post('/plants/moisture/low_alert/', low_alert)
    ]
)

if __name__ == '__main__':
    web.run_app(app, host='0.0.0.0', port=config['api']['port'])