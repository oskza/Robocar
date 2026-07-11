#include "CommandService.h"
#include "json/CommandJsonParser.h"
#include "json/CommandJsonSerializer.h"
#include "CommandResponseBuilder.h"

CommandService::CommandService(Robot &robot) : _dispatcher(robot) {}

bool CommandService::handle(const char *request, size_t requestLength, char *responseBuffer, size_t responseCapacity) {
    CommandEnvelope command{};
    CommandResponse response{};
    if (!CommandJsonParser::parse(request, requestLength, command)) {
        CommandResponseBuilder::error(response, CommandError::INVALID_COMMAND);
        return CommandJsonSerializer::serialize(response, responseBuffer, responseCapacity);
    }
    _dispatcher.dispatch(command, response);
    return CommandJsonSerializer::serialize(response, responseBuffer, responseCapacity);
}
