function url_format(url,n)
	local new_url	=	"";
	if n < 100 then
		new_url	=	url .. (n + 1);
	end

	return new_url;
end

