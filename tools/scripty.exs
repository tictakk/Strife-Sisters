Mix.install([{:jason, "~> 1.0"}])

commands = %{
  "dialog" => 0,
  "response" => 1
}

commanders = %{
  "rei" => 0,
  "violet" => 1,
  "king" => 2
}

mapper = %{
  "commands"   => commands,
  "commanders" => commanders
}

{result, file} = case File.read("../script/script.json") do
  {:error, f} -> {false,f}
  {:ok, data} -> {true,data}
  _           -> {false,nil}
end

if result do
  out = File.open!("../script/out", [:write])
  [data] = Jason.decode!(file)

  count = data
  |> Enum.reduce(0, fn d,i ->
    case d["command"] do
      "dialog" ->
        c_id = Map.fetch!(mapper["commands"],d["command"])
        p_id = Map.fetch!(mapper["commanders"],d["portrait"])#IO.binwrite(out,mapper["commands"])
        text = d["text"] <> <<0>>
        IO.binwrite(out,[c_id,p_id,text])
        i+1
      _        -> i

    end
  end)
  IO.puts "complete"
  IO.puts "number of lines written #{count}"
  File.close(file)
  File.close(out)
end
