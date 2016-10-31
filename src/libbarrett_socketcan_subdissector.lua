--[[
  Wireshark dissector for the Barrett Technologies CANBus protocol over SocketCAN.

  Author: Clint Liddick <cliddick@cmu.edu>
  Date: 10/25/2016
--]]

do
  local bit = require('bit')

  local barrett_can_proto = Proto("BARRETT_CAN", "Barrett Technologies CANBus protocol data dissector")

  -- Barrett-specific fields
  local f_is_group_id = ProtoField.bool("barrett_can.is_group_id", "Is Group ID")
  local f_from_addr = ProtoField.uint8("barrett_can.from_addr", "FROM address", base.DEC)
  local f_to_addr = ProtoField.uint8("barrett_can.to_addr", "TO address", base.DEC)
  local f_to_group = ProtoField.string("barrett_can.to_group", "TO group", "Text")
  local f_request_type = ProtoField.string("barrett_can.request_type", "Request type", "Text")
  local f_request_prop_id = ProtoField.uint8("barrett_can.request_prop_id", "Request property ID", base.DEC)
  local f_request_prop = ProtoField.string("barrett_can.request_prop", "Request property", "Text")
  local f_value = ProtoField.string("barrett_can.value", "Payload value", "Text")
  local f_P = ProtoField.int32("barret_can.P", "Motor")
  local f_JP = ProtoField.int32("barret_can.JP", "Encoder")
  local f_taring = ProtoField.bool("barrett_can.taring", "Taring")
  local f_force_x = ProtoField.float("barrett_can.force_x", "Force_x (N)")
  local f_force_y = ProtoField.float("barrett_can.force_y", "Force_y (N)")
  local f_force_z = ProtoField.float("barrett_can.force_z", "Force_z (N)")
  local f_torque_x = ProtoField.float("barrett_can.torque_x", "Torque_x (Nm)")
  local f_torque_y = ProtoField.float("barrett_can.torque_y", "Torque_y (Nm)")
  local f_torque_z = ProtoField.float("barrett_can.torque_z", "Torque_z (Nm)")
  local f_accel_x = ProtoField.float("barrett_can.accel_x", "Acceleration_x (m/s^2)")
  local f_accel_y = ProtoField.float("barrett_can.accel_y", "Acceleration_y (m/s^2)")
  local f_accel_z = ProtoField.float("barrett_can.accel_z", "Acceleration_z (m/s^2)")

  barrett_can_proto.fields = {
    f_is_group_id,
    f_from_addr,
    f_to_addr,
    f_to_group,
    f_request_type,
    f_request_prop_id,
    f_request_prop,
    f_value,
    f_P,
    f_JP,
    f_taring,
    f_force_x,
    f_force_y,
    f_force_z,
    f_torque_x,
    f_torque_y,
    f_torque_z,
    f_accel_x,
    f_accel_y,
    f_accel_z,
  }

  -- parent fields to be read
  local f_can_id = Field.new("can.id")
  local f_can_len = Field.new("can.len")

  function dissect_barrett_can(tvb, pinfo, tree)
    pinfo.cols.protocol = "BARRETT_CAN"
    local subtree = tree:add(barrett_can_proto,tvb(), "Barrett Message")

    -- 11th bit of ID determines whether 'TO' field refers to ID or group
    local id = f_can_id().value
    local group_bit = bit.band(id, 0x400)
    local is_group_id
    if group_bit == 0 then
      is_group_id = false
    else
      is_group_id = true
    end
    subtree:add(f_is_group_id, tvb(), is_group_id)

    -- bits [10,6] are 'FROM' address
    local from_addr = bit.band(id, 0x3E0)
    from_addr = bit.rshift(from_addr, 5)
    subtree:add(f_from_addr, tvb(), from_addr)

    -- bits [5,1] are 'TO' address
    local to_addr = bit.band(id, 0x1F)
    subtree:add(f_to_addr, tvb(), to_addr)

    -- if Group flag is set, 'TO' address is a GroupID
    if is_group_id then
      subtree:add(f_to_group, tvb(), get_group_string(to_addr))
    end

    -- first bit of first byte specifies request type
    local request_type_bit = bit.band(tvb(0,1):uint(), 0x80)

    function is_set_prop()
      return request_type_bit == 0x80
    end

    local request_type
    if is_set_prop() then
      request_type = 'SET PROPERTY'
    else
      request_type = 'GET PROPERTY'
    end
    subtree:add(f_request_type, tvb(0,1), request_type)

    -- last 7 bits of first byte specifies property requested
    local property_id = bit.band(tvb(0,1):uint(), 0x7F)
    subtree:add(f_request_prop_id, tvb(0,1), property_id)
    -- parse special F/T puck properties first
    if is_group_id and (to_addr == 10 or to_addr == 11) then
      subtree:add(f_request_prop, tvb(0,1), get_ft_property(property_id))
    else
      -- else parse as regular puck
      subtree:add(f_request_prop, tvb(0,1), get_puck_property(property_id))
    end

    -- parse payload
    local data_len = f_can_len().value
    local value = '0x0'

    if is_group_id and is_packed_group(to_addr) and is_set_prop() then
      -- parse packed position property

      -- parse first packed value
      local first = 0
      local M = tvb(0,1):int()
      M = bit.band(M, 0x3F)
      -- not a bug, actually Big Endian
      first = first + bit.lshift(M, 16)
      first = first + bit.lshift(tvb(1,1):int(), 8)
      first = first + tvb(2,1):int()

      -- parse second packed value if exists
      local second = 0
      if data_len == 6 then
        M = tvb(3,1):int()
        M = bit.band(M, 0x3F)
        second = second + bit.lshift(M, 16)
        second = second + bit.lshift(tvb(4,1):int(), 8)
        second = second + tvb(5,1):int()
      end

      if to_addr == 3 or to_addr == 7 then
        -- if position request
        subtree:add(f_P, tvb(0,3), first)
        if data_len == 6 then
          -- if position request with additional encoders
          subtree:add(f_JP, tvb(3,3), second)
        end
      elseif to_addr == 1 or to_addr == 2 then
        -- if torque request (not Force/Torque sensor)
        -- don't know how torques are packed so just print the hex for now
        subtree:add(f_value, tvb(), '0x' .. tvb:bytes():tohex())
      end

    elseif is_group_id and is_ft_group(to_addr) and is_set_prop() then
      -- parse packed Force/Torque property
      local prop = get_ft_property(property_id)
      if prop == 'FT' then
        -- force/torque
        if from_addr == 0 then
          -- any write is a tare
          subtree:add(f_taring, tvb(), true)
        else
          local x = tvb(0,2):le_int()
          local y = tvb(2,2):le_int()
          local z = tvb(4,2):le_int()
          if to_addr == 10 then
            -- parse force values
            subtree:add(f_force_x, tvb(0,2), x/256)
            subtree:add(f_force_y, tvb(2,2), y/256)
            subtree:add(f_force_z, tvb(4,2), z/256)
          else
            -- parse torque values
            subtree:add(f_torque_x, tvb(0,2), x/4096)
            subtree:add(f_torque_y, tvb(2,2), y/4096)
            subtree:add(f_torque_z, tvb(4,2), z/4096)
          end
        end
      elseif prop == 'A' then
        -- parse acceleration values
        local x = tvb(0,2):le_int()
        local y = tvb(2,2):le_int()
        local z = tvb(4,2):le_int()
        subtree:add(f_accel_x, tvb(0,2), x/1024)
        subtree:add(f_accel_x, tvb(2,2), y/1024)
        subtree:add(f_accel_x, tvb(4,2), z/1024)
      end
    else
      -- don't know the format
      if data_len == 4 or data_len == 6 then
        subtree:add(f_value, tvb(2,-1), tostring(tvb(2,-1):le_int()))
      else
        subtree:add(f_value, tvb(), '0x' .. tvb:bytes():tohex())
      end
    end
  end -- dissector function

  barrett_can_proto.dissector = dissect_barrett_can

  local can_dissector_table = DissectorTable.get("can.subdissector")
  can_dissector_table:add_for_decode_as(barrett_can_proto)


  function is_packed_group(gid)
    -- TODO all together?
    return gid == 1 or gid == 2 or gid == 3 or gid == 7
  end


  function is_ft_group(gid)
    return gid == 10 or gid == 11
  end


  function get_group_string(group_id)
    -- GroupIDs specified in Barrett's CAN_Message_Format.pdf
    local NUM_GROUPS = 12
    local GROUPS = {
      'ALL PUCKS',
      '4DOF TORQUES',
      'WRIST TORQUES',
      'POSITION FEEDBACK',
      'WHOLE WAM',
      'WHOLE BARRETTHAND',
      'PROPERTY FEEDBACK',
      'SECONDARY ENCODER FEEDBACK',
      'TACTILE TOP10',
      'TACTILE FULL',
      'FORCE/TORQUE SENSOR FORCE',
      'FORCE/TORQUE SENSOR TORQUE'
    }
    assert(#GROUPS == NUM_GROUPS, 'a group was accidentially deleted or missed from the list')
    if group_id >= NUM_GROUPS then return 'UNKNOWN GROUP' end
    return GROUPS[group_id + 1]
  end


  function get_ft_property(id)
    -- Force/Torque property IDs specified in ForceTorqueProperties.doc
    local NUM_FT_PROPS = 23
    local FT_PROPS_START = 42
    local FT_PROPS = {
      'SG1',
      'SG2',
      'SG3',
      'SG4',
      'SG5',
      'SG6',
      'FX',
      'FY',
      'FZ',
      'TX',
      'TY',
      'TZ',
      'FT',
      'AX',
      'AY',
      'AZ',
      'GM',
      'OV',
      'LED',
      'T1',
      'T2',
      'T3',
      'A'
    }
    assert(#FT_PROPS == NUM_FT_PROPS, 'a F/T  property was accidentially deleted or missed from the list')
    if id >= (FT_PROPS_START + NUM_FT_PROPS) or id < FT_PROPS_START then
      -- error(tostring(id) .. ' not valid F/T Property ID')
      return get_puck_property(id)
    else
      return FT_PROPS[id - FT_PROPS_START + 1]
    end
  end


  function get_puck_property(id)
    -- Puck property IDs specified in Barrett's PuckProperties.pdf
    local NUM_PUCK_PROPS = 109
    local PUCK_PROPS = {
      'VERS',
      'ROLE',
      'SN',
      'ID',
      'ERROR',
      'STAT',
      'ADDR',
      'VALUE',
      'MODE',
      'TEMP',
      'PTEMP',
      'OTEMP',
      'BAUD',
      'LOCK',
      'DIG0',
      'DIG1',
      'FET0',
      'FET1',
      'ANA0',
      'ANA1',
      'THERM',
      'VBUS',
      'IMOTOR',
      'VLOGIC',
      'ILOGIC',
      'SG',
      'GRPA',
      'GRPB',
      'GRPC',
      'CMD',
      'SAVE',
      'LOAD',
      'DEF',
      'FIND',
      'X0',
      'X1',
      'X2',
      'X3',
      'X4',
      'X5',
      'X6',
      'X7',
      'T',
      'MT',
      'V',
      'MV',
      'MCV',
      'MOV',
      'P',
      'P2',
      'DP',
      'DP2',
      'E',
      'E2',
      'OT',
      'OT2',
      'CT',
      'CT2',
      'M',
      'M2',
      'DS',
      'MOFST',
      'IOFST',
      'UPSECS',
      'OD',
      'MDS',
      'MECH',
      'MECH2',
      'CTS',
      'CTS2',
      'PIDX',
      'HSG',
      'LSG',
      'IVEL',
      'IOFF',
      'IOFF2',
      'MPE',
      'HOLD',
      'TSTOP',
      'KP',
      'KD',
      'KI',
      'ACCEL',
      'TENST',
      'TENSO',
      'JIDX',
      'IPNM',
      'HALLS',
      'HALLH',
      'HALLH2',
      'POLES',
      'IKP',
      'IKI',
      'IKCOR',
      'EN',
      'EN2',
      'JP',
      'JP2',
      'JOFST',
      'JOFST2',
      'TIE',
      'ECMAX',
      'ECMIN',
      'LFLAGS',
      'LCTC',
      'LCVC',
      'TACT',
      'TACTID',
      'IHIT'
    }

    assert(#PUCK_PROPS == NUM_PUCK_PROPS, "A property was accidentially deleted or missed from the list")
    if id >= NUM_PUCK_PROPS then return 'INVALID PROP ID ' .. tostring(id) end

    return PUCK_PROPS[id+1]
  end

end
