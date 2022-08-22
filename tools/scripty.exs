Mix.install([{:jason, "~> 1.0"}])

defmodule Formatter do
  @require 4

  @commands %{
    "dialog" => 0,
    "sound" => 1,
    "walk" => 2,
    "end" => 3
  }

  @units %{
    "rei" => 16,
    "violet" => 17,
    "king" => 18,
    "empty" => 0
  }

  @mapper %{
    "commands"   => @commands,
    "units"      => @units
  }

  @game_state %{
    "prebattle" => 0xFE,
    "inbattle" => 0xFD,
    "postbattle" => 0xFC,
    "explore" => 0xFB
  }

  def format(data,out) do
    count = data
    |> Enum.reduce(0, fn d,i ->
      case d["command"] do
        "dialog" ->
          c_id = Map.fetch!(@mapper["commands"],d["command"])
          p_id = Map.fetch!(@mapper["units"],d["portrait"])
          text = d["text"] <> <<0>>
          IO.binwrite(out,[c_id,p_id,text])
          i+1

        "end"    ->
          IO.binwrite(out,[0,3])
          i+1
        _        -> i
      end
    end)
    IO.puts "complete"
    IO.puts "number of lines written #{count}"
  end

  def format_area(data,out) do
    area_num = Map.get(data,"area")
    IO.binwrite(out,[0xFF,area_num])

    Enum.each(Map.keys(@game_state),
    fn k ->
      case length(Map.get(data,k)) do
        0 -> IO.binwrite(out,[Map.get(@game_state,k)])
        any ->
          IO.binwrite(out,[Map.get(@game_state,k)])
          case k do
            "explore" -> explore(Map.get(data,k),out)
            _         -> format(Map.get(data,k),out)
          end
      end
    end
    )
  end

  def explore(data,out) do
    Enum.each(data,
    fn d ->
      IO.binwrite(out,[@require,d["id"]])
      format(d["npc"],out)
    end
    )
  end
end

{result, file} = case File.read("../script/script.json") do
  {:error, f} -> {false,f}
  {:ok, data} -> {true,data}
  _           -> {false,nil}
end

if result do
  out = File.open!("../script/out", [:write])

  json = Jason.decode!(file)
  # data = List.first(json)
  # Formatter.format(Map.get(data,"prebattle"),out)
  # Formatter.format_area(data,out)
  # data = List.last(json)
  # Formatter.format_area(data,out)
  Enum.each(json, fn area -> Formatter.format_area(area,out) end)

  File.close(file)
  File.close(out)
end
