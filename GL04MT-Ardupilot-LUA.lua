local SCRIPT_NAME          = 'Depthfinder'
local MAV_DEPTH_LABEL      = 'DEPTH'
local BOOT_DELAY_MS        = 2000
local RUN_INTERVAL_MS      =  50
local SERIAL_PORT          =    0
local BAUD_RATE            = 115200
local FRAME_SIZE           =    4
local SPEED_RATIO          =    1
local INVALID_DEPTH        =   -1
local MAV_SEVERITY_WARNING =    4
local MAV_SEVERITY_INFO    =    6
local port = serial:find_serial(SERIAL_PORT)

port:begin(BAUD_RATE)
port:set_flow_control(0)

local function serial_flush(bytes)
    for i = 0, bytes - 1 do
        port:read()
    end
end

function update()
    local bytes_available = port:available():toint()

    if bytes_available ~= FRAME_SIZE then
        serial_flush(bytes_available)
        gcs:send_named_float(MAV_DEPTH_LABEL, INVALID_DEPTH)
        return update, RUN_INTERVAL_MS
    end

    local data = 0
    for i = 0, bytes_available - 1 do
        data = data << 8 | port:read()
    end

    local sum    = data & 0xFF
    local l_data = data >>  8 & 0xFF
    local h_data = data >> 16 & 0xFF

    if (0xFF + h_data + l_data) & 0xFF ~= sum then
        gcs:send_text(MAV_SEVERITY_WARNING,
                      string.format('%s: Bad checksum', SCRIPT_NAME))
        gcs:send_named_float(MAV_DEPTH_LABEL, INVALID_DEPTH)
        return update, RUN_INTERVAL_MS
    end

    local depth_raw = h_data << 8 | l_data
    gcs:send_named_float('DEPTH', depth_raw)
        if terrainSensor ~= nil then
            terrainSensor:handle_script_msg(depth_raw / 100)
            gcs:send_text(6,"rangefinder")
        end
    return update, RUN_INTERVAL_MS
end

gcs:send_text(MAV_SEVERITY_INFO, string.format('%s: Script Active', SCRIPT_NAME))

return update, BOOT_DELAY_MS
