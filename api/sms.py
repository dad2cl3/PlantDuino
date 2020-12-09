import boto3, json, os

with open ('config.json', 'r') as config_file:
    config = json.load(config_file)

def publish():
    os.environ['AWS_PROFILE'] = config['aws']['profile']
    os.environ['AWS_DEFAULT_REGION'] = config['aws']['region']

    client = boto3.client('sns')

    response = client.publish(
        TopicArn=config['sns']['topicArn'],
        Message=config['sns']['message'],
        Subject=config['sns']['subject'],
        MessageStructure=config['sns']['structure']
    )

    return response